<?php

class DeployProcessorBatch extends DeployProcessorMemory {

  function process(DeployPlan $plan, $endpoint_name) {
    $iterator = $this->aggregator->getIterator();
    // Set up batch processing of the users...
    $operations = array();
    foreach ($iterator as $entity_wrapper) {
      // Create item
      $item = array(
        'plan' => $plan->name,
        'eid' => $entity_wrapper->getIdentifier(),
        'type' => $entity_wrapper->type(),
        'endpoint' => $endpoint_name,
      );
      $operations[] = array('deploy_queue_process_item', array($item));
    }
    $batch = array(
      'title' => t('Deploying entities'),
      'operations' => $operations,
    );
    batch_set($batch);
    batch_process('admin/structure/deploy');
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }
}
