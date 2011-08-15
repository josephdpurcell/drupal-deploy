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
    $this->config['headers']['Content-Type'] = 'application/json';
    $response = $this->httpPut($entity->uuid, $data, 'json');
    $plan = $this->plan;
    $args = array('eid' => $entity_wrapper->getIdentifier(), 'response' => $response, 'plan' => $plan->name);
    module_invoke_all('deploy_item_deployed', $this, $args);
  }
}
