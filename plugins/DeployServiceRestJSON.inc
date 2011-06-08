<?php

class DeployServiceRestJSON extends DeployServiceRest {

  function deploy(Array $entity_wrappers) {
    foreach ($entity_wrappers as $entity_wrapper) {
      $entity = $entity_wrapper->value();
      $entity_type = $entity_wrapper->type();
      $entity_id = $entity_wrapper->getIdentifier();
      // To follow REST principles strictly, we can only post properties that
      // are writable. So we need to go through all of them.
      foreach ($entity as $key => $value) {
        $property_info = $entity_wrapper->getPropertyInfo();
        if (!isset($property_info[$key]) || !isset($property_info[$key]['setter callback'])) {
          unset($entity->{$key});
        }
      }
      $data = drupal_json_encode($entity);
      $this->httpPost($entity_type, $entity_id, $data, 'json');
    }
  }

}
