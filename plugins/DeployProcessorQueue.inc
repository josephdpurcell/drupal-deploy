<?php

/**
 * Processor class using the Batch API
 */
class DeployProcessorQueue extends DeployProcessorMemory {

  public function deploy($deployment_key, DeployEndpoint $endpoint, $lock_name = NULL) {
    deploy_log($deployment_key, DEPLOY_STATUS_PROCESSING);
    $queue = DrupalQueue::get('deploy_deploy');
    foreach ($this->aggregator as $entity) {
      // We don't want entities that has an origin.
      if (empty($entity->__metadata['origin'])) {
        $entity_info = entity_get_info($entity->__metadata['type']);
        $id_key = $entity_info['entity keys']['id'];

        $entity->__metadata['id'] = $entity->{$id_key};
        $entity->__metadata['deployment_key'] = $deployment_key;
        $entity->__metadata['plan_name'] = $this->aggregator->plan->name;
        $entity->__metadata['endpoint_name'] = $endpoint->name;
        $entity->__metadata['lock_name'] = $lock_name;
        $queue->createItem($entity);
      }
    }
  }

  public function publish($deployment_key, DeployEndpoint $endpoint, $lock_name = NULL) {
    $queue = DrupalQueue::get('deploy_publish');
    foreach ($this->aggregator as $entity) {
      // We don't want entities that has an origin.
      if (empty($entity->__metadata['origin'])) {
        $entity_info = entity_get_info($entity->__metadata['type']);
        $id_key = $entity_info['entity keys']['id'];

        $entity->__metadata['id'] = $entity->{$id_key};
        $entity->__metadata['deployment_key'] = $deployment_key;
        $entity->__metadata['plan_name'] = $this->aggregator->plan->name;
        $entity->__metadata['endpoint_name'] = $endpoint->name;
        $entity->__metadata['lock_name'] = $lock_name;
        $queue->createItem($entity);
      }
    }
  }
}
