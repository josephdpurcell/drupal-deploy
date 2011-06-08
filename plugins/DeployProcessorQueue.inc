<?php

class DeployProcessorQueue extends DeployProcessorMemory {

  function process($endpoint_name) {
    // TODO: Figure out how split up and queue the aggregator.
  }

  function configForm(&$form_state) {
    // We have no settings for this processor.
    return array();
  }

}
