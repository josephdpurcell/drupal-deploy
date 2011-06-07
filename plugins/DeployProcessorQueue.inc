<?php

class DeployProcessorQueue extends DeployProcessorMemory {

  function process($endpoint_name) {
    $iterator = $this->provider->getEntityIterator();
    // TODO: Figure out how split up and queue this.
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }

}
