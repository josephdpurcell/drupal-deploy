<?php

class DeployProcessorQueue extends DeployProcessorMemory {

  function process(DeployEndpoint $endpoint) {
    foreach ($this->provider->getEntities() as $entity) {
      // @todo Add resource and endpoint to queue.
    }
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }

}
