<?php

class DeployProcessorQueue extends DeployProcessorMemory {

  function process(DeployPlan $plan, $endpoint_name) {
    $iterator = $this->aggregator->getIterator();
    $deploy_queue = DrupalQueue::get('deploy_queue');
    foreach ($iterator as $entity_wrapper) {
      // Create item
      $item = array(
        'plan' => $plan->name,
        'eid' => $entity_wrapper->getIdentifier(),
        'type' => $entity_wrapper->type(),
        'endpoint' => $endpoint_name,
      );
      // Add item to queue
      $deploy_queue->createItem($item);
    }
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }
}
