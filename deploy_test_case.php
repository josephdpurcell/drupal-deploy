<?php

class DeployWebTestCase extends DrupalWebTestCase {

  // Array to keep presaved variables.
  private $servers = array();

  /**
   * Set up all sites.
   */
  function setUp() {
    $this->setUpSite('deploy_origin', array('entity', 'ctools', 'features', 'views', 'views_ui', 'uuid', 'deploy', 'deploy_ui', 'deploy_aggregator_views', 'deploy_example'));

    // Switch back to original site to be able to set up a new site.
    $this->switchSite('deploy_origin', 'simpletest_original_default');

    $this->setUpSite('deploy_endpoint', array('entity', 'ctools', 'features', 'uuid', 'services', 'rest_server', 'uuid_services', 'uuid_services_example'));

    // Switch back to origin site where we want to start.
    $this->switchSite('deploy_endpoint', 'deploy_origin');

    // We need to change the URL and set User Agent header in order to reach
    // the endpoint.
    $endpoint = deploy_endpoint_load('deploy_example_endpoint');
    $endpoint->service_config['url'] = url('api', array('absolute' => TRUE));
    $user_agent = drupal_generate_test_ua($this->sites['deploy_endpoint']->databasePrefix);
    $endpoint->service_config['headers'] = array('User-Agent' => $user_agent);
    ctools_export_crud_save('deploy_endpoints', $endpoint);
  }

  /**
   * Tear down all sites.
   *
   * @todo Make this transparent of how many sites we've set up.
   */
  function tearDown() {
    // Tear down current site.
    parent::tearDown();

    // Tear down the origin.
    file_unmanaged_delete_recursive($this->originalFileDirectory . '/simpletest/' . substr($this->sites['deploy_origin']->databasePrefix, 10));
    // Remove all tables and the connection.
    Database::setActiveConnection('deploy_origin');
    $schema = drupal_get_schema(NULL, TRUE);
    foreach ($schema as $name => $table) {
      db_drop_table($name);
    }
    Database::setActiveConnection('default');
    Database::removeConnection('deploy_origin');
  }

  /**
   * Set up a new site.
   */
  function setUpSite($key, $modules) {
    static $original = array();

    call_user_func_array(array($this, 'parent::setUp'), $modules);
    $this->saveState($key);

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
    Database::renameConnection('default', $from);
    Database::renameConnection($to, 'default');

    // No need to restore anything if we are switching to the original site.
    if ($to != 'simpletest_original_default') {
      $this->restoreState($to);
      drupal_static_reset();
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
}
