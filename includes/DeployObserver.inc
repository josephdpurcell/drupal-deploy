<?php

class DeployObserver {
  function onDeploy($sender, $args) {
    // Take actions based on the success or failure of a deployed entity
    module_invoke_all('deploy_item_deployed', $sender, $args);
  }
}