<?php

class DeployServiceRestJSON extends DeployServiceRest {

  function deploy(Traversable $iterator) {
    foreach ($iterator as $entity_wrapper) {
      $entity = $entity_wrapper->value();
      $data = drupal_json_encode(
        array(
          'type' => $entity_wrapper->type(),
          'data' => $entity,
        )
      );

      $this->setHeader('Content-Type', 'application/json');
      $this->httpPut($entity->uuid, $data, 'json');
    }
  }

}
