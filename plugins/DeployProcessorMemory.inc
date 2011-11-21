<?php

/**
 * Class for basic memory-based processing
 */
class DeployProcessorMemory implements DeployProcessor {

  /**
   * The aggregator containing the entities to be processed.
   *
   * @var DeployAggregator
   */
  public $aggregator = NULL;

  /**
   * Configuration options.
   *
   * @var array
   */
  public $config = array();

  public function __construct(DeployAggregator $aggregator, Array $config = array()) {
    $this->aggregator = $aggregator;
    $this->config += array(
      'debug' => FALSE,
    );
    $this->config = array_merge($this->config, $config);
  }

  protected function processOperations(Array $operations = array()) {
    if (!empty($operations)) {
      foreach ($this->aggregator as $data) {
        foreach ($operations as $operation) {
          $operation['callback']($this->aggregator->plan->name, $data);
        }
      }
    }
  }

  public function preProcess(Array $operations = array()) {
    $this->processOperations($operations);
  }

  public function deploy(DeployEndpoint $endpoint) {
    $endpoint->deploy($this->aggregator->plan, $this->aggregator);
    drupal_set_message(t('Plan %plan has been deployed to %endpoint.', array('%plan' => $this->aggregator->plan->title, '%endpoint' => $endpoint->title)));
  }

  public function publish(DeployEndpoint $endpoint) {
    $endpoint->publish($this->aggregator->plan, $this->aggregator);
  }

  public function postProcess(Array $operations = array()) {
    $this->processOperations($operations);
  }

  public function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }
}
