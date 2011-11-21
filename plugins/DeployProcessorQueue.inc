<?php

/**
 * Processor class using the Batch API
 */
class DeployProcessorQueue extends DeployProcessorMemory {

  public function deploy(DeployEndpoint $endpoint) {
    $queue = DrupalQueue::get('deploy_deploy');
    foreach ($this->aggregator as $data) {
      // Since we are queuing for unknown time, we only want to queue
      // originators. Their dependencies will be figured out anyhow when
      // processing later on.
      if ($data['relationship']['type'] == ENTITY_RELATIONSHIP_ORIGIN) {
        $data['plan_name'] = $this->aggregator->plan->name;
        $data['endpoint_name'] = $endpoint_name;
        $queue->createItem($data);
      }
      drupal_set_message(t('%plan has been queued for deployment to %endpoint.', array('%plan' => $this->aggregator->plan->title, '%endpoint' => $endpoint->title)));
    }
  }
}
