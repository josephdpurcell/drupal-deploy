<?php

class DeployProcessorBatch extends DeployProcessorMemory {

  function process(DeployPlan $plan, $endpoint_name) {
    $iterator = $this->aggregator->getIterator();
    $batch_func = 'DeployProcessorBatch::process_item_batch';
    $batch_title = t('Deploying Entities');
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
      $operations[] = array($batch_func, array($item));

    }
    $batch = array(
      'title' => $batch_title,
      'operations' => $operations,
    );
    batch_set($batch);
    batch_process('admin/structure/deploy/plans');
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }

  static function process_item_batch($item, &$context) {
    self::process_item($item);
    $context['results'][] = 'Deployed item ' . $item['eid'];
    $context['message'] = t('Deploying item @item', array('@item' => $item['eid']));
  }

}
