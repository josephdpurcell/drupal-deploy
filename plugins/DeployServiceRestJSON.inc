<?php

class DeployServiceRestJSON extends DeployServiceRest {

  function deploy(Traversable $iterator) {
    foreach ($iterator as $entity_wrapper) {
      $entity = $entity_wrapper->value();
      $entity_type = $entity_wrapper->type();
      $entity_id = $entity_wrapper->getIdentifier();
      // To follow REST principles strictly, we can only post properties that
      // are writable. So we need to go through all of them.
      foreach ($entity as $key => $value) {
        $property_info = $entity_wrapper->getPropertyInfo();
        // If a property should be deployed with the entity, it needs to be
        // defined in hook_entity_property_info() with a 'setter callback'. This
        // means that the property is writable.
        //
        // TODO: Should we apply this on a higher level in Deploy, and not just
        // in the REST service?
        if (!isset($property_info[$key]) || !isset($property_info[$key]['setter callback'])) {
          unset($entity->{$key});
        }
      }
      $data = drupal_json_encode($entity);
      $this->httpPost($entity_type, $entity_id, $data, 'json');
    }
  }

}
