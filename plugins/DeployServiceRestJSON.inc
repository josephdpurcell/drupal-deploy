<?php

class DeployServiceRestJSON extends DeployServiceRest {

  function deploy(Traversable $iterator) {
    foreach ($iterator as $entity_wrapper) {
      $this->deployEntity($entity_wrapper);
    }
  }

  function deployEntity($entity_wrapper) {
    $entity = $entity_wrapper->value();
    $data = drupal_json_encode(
      array(
        'type' => $entity_wrapper->type(),
        'data' => $entity,
      )
    );
    $this->setHeader('Content-Type', 'application/json');
    $response = $this->httpPut($entity->uuid, $data, 'json');
    // TODO: need to get the plan name and pass it as part of the args...
    $plan = deploy_active();
    $args = array('eid' => $entity_wrapper->getIdentifier(), 'response' => $response, 'plan' => $plan);
    foreach ($this->observers as $observer) {
      $observer->onDeploy($this, $args);
    }
  }
}
