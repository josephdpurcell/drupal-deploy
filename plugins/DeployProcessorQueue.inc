<?php

/**
 * Processor class using the Batch API
 */
class DeployProcessorQueue extends DeployProcessorMemory {

  public function process($endpoint_name) {
    if ($endpoint = deploy_endpoint_load($endpoint_name)) {
      $iterator = $this->aggregator->getIterator();
      $queue = DrupalQueue::get('deploy_queue');

      foreach ($iterator as $entity_wrapper) {
        $item = array(
          'plan' => $this->aggregator->plan->name,
          'eid' => $entity_wrapper->getIdentifier(),
          'type' => $entity_wrapper->type(),
          'endpoint' => $endpoint_name,
        );
        $queue->createItem($item);
      }
      drupal_set_message(t('Plan %plan has been queued for deployment to %endpoint.', array('%plan' => $this->aggregator->plan->title, '%endpoint' => $endpoint->title)));
    }
  }

  public function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }
}
