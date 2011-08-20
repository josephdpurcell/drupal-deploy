<?php

class DeployProcessorBatch extends DeployProcessorMemory {

  function process($endpoint_name) {
    if ($endpoint = deploy_endpoint_load($endpoint_name)) {
      $iterator = $this->aggregator->getIterator();
      $operations = array();

      foreach ($iterator as $entity_wrapper) {
        $item = array(
          'plan' => $this->aggregator->plan->name,
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
    }
  }

  function cleanup() {
    drupal_set_message(t('Plan %plan has been deployed to its endpoints', array('%plan' => $this->aggregator->plan->title)));
    batch_process('admin/structure/deploy');
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }
}
