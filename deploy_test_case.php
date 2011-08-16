<?php

class DeployWebTestCase extends DrupalWebTestCase {

  // Array to keep presaved variables.
  private $servers = array();

  /**
   * Set up all sites.
   *
   * For some tests we don't need the multisite environment, but still want
   * to use common methods in this test case.
   */
  function setUp($simple = FALSE) {
    $this->profile = 'standard';
    if ($simple) {
      parent::setUp('entity', 'deploy');
      return;
    }

    // Set up our origin site.
    $this->setUpSite('deploy_origin', array('entity', 'ctools', 'features', 'views', 'views_ui', 'uuid', 'deploy', 'deploy_ui', 'deploy_aggregator_views', 'deploy_example'));

    // Switch back to original site to be able to set up a new site.
    $this->switchSite('deploy_origin', 'simpletest_original_default');

    // Set up one endpoint site.
    $this->setUpSite('deploy_endpoint', array('entity', 'ctools', 'features', 'uuid', 'services', 'rest_server', 'uuid_services', 'uuid_services_example'));

    // Switch back to origin site where we want to start.
    $this->switchSite('deploy_endpoint', 'deploy_origin');

    // Edit an endpoint object to point to our endpoint site.
    $this->editEndpoint('deploy_example_endpoint', 'deploy_endpoint');
  }

  /**
   * Tear down all sites.
   *
   * @todo Make this transparent of how many sites we've set up.
   */
  function tearDown() {
    // Tear down current site.
    parent::tearDown();
    // We are making it easy for us (but a bit hacky) by using this method to
    // clean out other environments that we've created.
    simpletest_clean_database();
    simpletest_clean_temporary_directories();
    registry_rebuild();
    cache_clear_all('simpletest', 'cache');
  }

  /**
   * Set up a new site.
   */
  function setUpSite($key, $modules) {
    static $original = array();

    call_user_func_array(array($this, 'parent::setUp'), $modules);
    $this->saveState($key);
    variable_set('deploy_site_hash', md5(time()));

    // Save original settings after first setUp(). We need to be able to restore
    // them after subsequent calls to setUp().
    if (empty($original)) {
      $original = array(
        $this->originalLanguage,
        $this->originalLanguageDefault,
        $this->originalFileDirectory,
        $this->originalProfile,
        $this->originalShutdownCallbacks,
      );
    }

    // Restore the original settings.
    list(
      $this->originalLanguage,
      $this->originalLanguageDefault,
      $this->originalFileDirectory,
      $this->originalProfile,
      $this->originalShutdownCallbacks
    ) = $original;
  }

  /**
   * Switch to a specific site.
   */
  function switchSite($from, $to) {
    // This is used to test the switch.
    $old_site_hash = variable_get('deploy_site_hash', '');

    // Switch database connection. This is where the magic happens.
    Database::renameConnection('default', $from);
    Database::renameConnection($to, 'default');

    // Reset static caches, so sites doesn't share them.
    drupal_static_reset();
    // Since variables ($conf) lives in the global namespace, we need to
    // reinitalize them to not make sites share variables.
    cache_clear_all('*', 'cache_bootstrap');
    $GLOBALS['conf'] = variable_initialize();

    // No need to restore anything if we are switching to the original site.
    if ($to != 'simpletest_original_default') {
      $this->restoreState($to);

      // Test the switch.
      $new_site_hash = variable_get('deploy_site_hash', '');
      $this->assertNotEqual($old_site_hash, $new_site_hash, t('Switch to site %key was successful.', array('%key' => $to)));
    }
  }

  /**
   * Save state.
   */
  function saveState($key) {
    $this->sites[$key]->cookieFile = $this->cookieFile;
    $this->sites[$key]->databasePrefix = $this->databasePrefix;
    $this->sites[$key]->curlHandle = $this->curlHandle;
    $this->sites[$key]->cookieFile = $this->cookieFile;
  }

  /**
   * Restore state.
   */
  function restoreState($key) {
    $this->cookieFile = $this->sites[$key]->cookieFile;
    $this->databasePrefix = $this->sites[$key]->databasePrefix;
    $this->curlHandle = $this->sites[$key]->curlHandle;
    $this->cookieFile = $this->sites[$key]->cookieFile;
  }

  /**
   * Edit an endpoint to make it point to a specific site in this test
   * environment.
   *
   * This is needed in order for deployments to be able to reach sites in this
   * test environment.
   */
  function editEndpoint($endpoint_name, $site_key) {
    $endpoint = deploy_endpoint_load($endpoint_name);
    $endpoint->service_config['url'] = url('api', array('absolute' => TRUE));
    $user_agent = drupal_generate_test_ua($this->sites[$site_key]->databasePrefix);
    $endpoint->service_config['headers'] = array('User-Agent' => $user_agent);
    ctools_export_crud_save('deploy_endpoints', $endpoint);
  }

  /**
   * Deploy the plan.
   *
   * @param string $name
   *   Name of the deployment plan.
   * @return type
   */
  function deployPlan($name) {
    if (empty($name)) {
      return;
    }
    $deployment_plan = deploy_plan_load($name);
    $deployment_plan->deploy();
  }

  /**
   * Code taken from TaxonomyWebTestCase::createTerm() since we can't extend
   * that test case. Some simplifications are made though.
   *
   * @todo
   *   This will probably not work when the Testing profile is used. Then we
   *   need to create the vocabulary manually.
   *
   * @see TaxonomyWebTestCase::createTerm()
   */
  function createTerm() {
    $term = new stdClass();
    $term->name = $this->randomName();
    $term->description = $this->randomName();
    // Use the first available text format.
    $term->format = db_query_range('SELECT format FROM {filter_format}', 0, 1)->fetchField();
    // For our test cases it's enough to rely on the standard 'tags' vocabulary.
    $term->vid = 1;
    taxonomy_term_save($term);
    return $term;
  }
}
