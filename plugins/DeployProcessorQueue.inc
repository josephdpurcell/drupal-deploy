<?php

class DeployProcessorQueue extends DeployProcessorMemory {

  function process($endpoint_name) {
    foreach ($this->provider->getEntityIds() as $entity) {
      // @todo Add resource id and endpoint name to queue.
    }
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }

}
