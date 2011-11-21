<?php

/**
 * Exception raised by malformed deployment plans.
 */
class DeployPlanMalformedException extends Exception {}

/**
 * Exception raised by malformed deployment plans.
 */
class DeployPlanRunningException extends Exception {}

/**
 * Class representing a deployment plan.
 */
class DeployPlan {

  /**
   * Whether debug mode is enabled for this plan.
   *
   * @var boolean
   */
  public $debug = FALSE;

  /**
   * The name of the aggregator plugin to be used to aggregate items for this
   * plan.
   *
   * @var string
   */
  public $aggregator_plugin = NULL;

  /**
   * An associative array of configuration settings to pass to the aggregator
   * object's constructor. Allowable keys will depend on the plugin being used.
   *
   * @var array
   */
  public $aggregator_config = array();

  /**
   * The deploy aggregator object that will be used to aggregate items for
   * deployment.
   *
   * @var DeployAggregator
   */
  public $aggregator = NULL;

  /**
   * The name of the processor plugin to be used to process this plan.
   *
   * @var string
   */
  public $processor_plugin = NULL;

  /**
   * An associative array of configuration settings to pass to the processor
   * object's constructor. Allowable keys will depend on the plugin being used.
   *
   * @var array
   */
  public $processor_config = array();

  /**
   * The processor object that will be used to process this plan.
   *
   * @var DeployProcessor
   */
  public $processor = NULL;

  /**
   * An array of names of endpoints that this plan is to be deployed to.
   *
   * @var array
   */
  public $endpoints = array();

  /**
   * Load the plan with its aggregator and processor.
   *
   * Since the CTools Export API is declarative by nature, we can't rely on
   * constructor injection and deploy_plan_create() as the only factory.
   */
  public function load() {
    $schema = drupal_get_schema('deploy_plans');
    // Unserialize our serialized params.
    foreach ($schema['fields'] as $field => $info) {
      if (!empty($info['serialize']) && !is_array($this->{$field})) {
        $this->{$field} = (array)unserialize($this->{$field});
      }
    }
    if (!empty($this->aggregator_plugin)) {
      $aggregator_config = $this->aggregator_config + array('debug' => $this->debug);
      $this->aggregator = new $this->aggregator_plugin($this, $aggregator_config);
    }
    if (!empty($this->processor_plugin)) {
      $processor_config = $this->processor_config + array('debug' => $this->debug);
      $this->processor = new $this->processor_plugin($this->aggregator, $processor_config);
    }
  }

  /**
   * Deploy the plan.
   */
  public function deploy() {
    if (empty($this->processor)) {
      $this->load();
    }
    if (empty($this->processor) || $this->fetch_only) {
      throw new DeployPlanMalformedException(t("The plan %plan can't be deployed in push fashion because it's missing a processor plugin or is <em>fetch-only</em>.", array('%plan' => $this->name)));
    }
    if (empty($this->endpoints)) {
      throw new DeployPlanMalformedException(t("The plan %plan can't be deployed in push fashion because it needs at least one endpoint to deploy to", array('%plan' => $this->name)));
    }

    // We only allow one deployment of each plan at the time.
    $lock_name = 'deploy_plan_' . $this->name;
    if (!lock_aquire($lock_name)) {
      throw new DeployPlanRunningException(t('A deployment of %plan is already running.', array('%plan' => $this->name)));
    }

    try {
      $deployment_key = deploy_deployment_log(NULL, DEPLOY_STATUS_STARTED, $this->name);

      // Allow other modules to do some preprocessing.
      $operations = module_invoke_all('deploy_preprocess_operations');
      $this->processor->preProcess($operations);

      // We deploy to all endpoints first, then we publish the deployments. This
      // will keep higher data consistency across all endpoints.
      $endpoints = array();
      foreach ($this->endpoints as $endpoint_name) {
        if ($endpoint = deploy_endpoint_load($endpoint_name)) {
          $endpoints[] = $endpoint;
          $this->processor->deploy($lock_name, $deployment_key, $endpoint);
        }
        else {
          throw new DeployPlanException(t("The plan %plan can't be deployed because the endpoint %endpoint is invalid.", array('%plan' => $this->name, '%endpoint' => $endpoint_name)));
        }
      }
      foreach ($endpoints as $endpoint) {
        $this->processor->publish($endpoint);
      }

      // Allow other modules to do some postprocessing.
      $operations = module_invoke_all('deploy_postprocess_operations');
      $this->processor->postProcess($operations);
    }
    catch (Exception $e) {
      // We have to release the lock if any exceptions where thrown.
      lock_release($lock_name);
      // Log the deployment as failed.
      deploy_deployment_log($deployment_key, DEPLOY_STATUS_FAILED);
      throw $e;
    }
  }

  /**
   * Returns the entities to be deployed by this plan.
   *
   * @return array()
   *   An array of entities structured as follows:
   *   @code
   *     $entities = array(
   *       'node' => array(
   *         10 => TRUE,
   *         12 => array(
   *           'taxonomy_term' => array(
   *             14 => TRUE,
   *           ),
   *           'user' => array(
   *             8 => TRUE,
   *           ),
   *         ),
   *       ),
   *       'taxonomy_term' => array(
   *         16 => TRUE,
   *       ),
   *     );
   *   @endcode
   */
  public function getEntities() {
    if (empty($this->aggregator)) {
      $this->init();
    }
    return $this->aggregator->getEntities();
  }

  /**
   * Returns an object that implements the DeployAggregator interface.
   *
   * @return DeployAggregator
   */
  public function getIterator() {
    if (empty($this->aggregator)) {
      $this->init();
    }
    return $this->aggregator;
  }
}
