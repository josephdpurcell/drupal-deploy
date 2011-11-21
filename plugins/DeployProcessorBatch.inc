<?php

/**
 * Processor class using the Batch API
 */
class DeployProcessorBatch extends DeployProcessorMemory {

  /**
   * Batch operations to execute.
   *
   * @var array
   */
  public $operations = array();

  protected function processOperations(Array $operations = array()) {
    if (!empty($operations)) {
      foreach ($this->aggregator as $data) {
        foreach ($operations as $operation) {
          $this->operations[] = array($operation['callback'], array($this->aggregator->plan->name, $item));
        }
      }
    }
  }

  public function deploy(DeployEndpoint $endpoint) {
    foreach ($this->aggregator as $data) {
      $this->operations[] = array('deploy_batch_operation_deploy', array($this->aggregator->plan->name, $endpoint->name, $data));
    }
  }

  public function publish(DeployEndpoint $endpoint) {
    foreach ($this->aggregator as $data) {
      $this->operations[] = array('deploy_batch_operation_publish', array($this->aggregator->plan->name, $endpoint->name, $data));
    }
  }

  public function postProcess(Array $operations = array()) {
    $this->processOperations($operations);
    $batch = array(
      'title' => t('Deploying %plan to %endpoint', array('%plan' => $this->aggregator->plan->title, '%endpoint' => $endpoint->title)),
      'operations' => $this->operations,
    );
    batch_set($batch);
    drupal_set_message(t('%plan has been deployed to its endpoints', array('%plan' => $this->aggregator->plan->title)));
    batch_process('admin/structure/deploy');
  }
}
