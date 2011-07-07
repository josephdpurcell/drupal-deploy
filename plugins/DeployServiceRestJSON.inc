<?php

class DeployServiceRestJSON extends DeployServiceRest {

  function deploy(Traversable $iterator) {
    foreach ($iterator as $entity_wrapper) {
      $entity = $entity_wrapper->value();
      // All IDs will later be stripped away. See below.
      $uuid = $entity->uuid;
      if (isset($entity->vuuid)) {
        $vuuid = $entity->vuuid;
      }

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
      // We need to set the entity_type on the object we're sending, otherwise
      // there's no way to tell on the other end what type of entity this is.
      // TODO: Does this violate the above?
      $entity->entity_type = $entity_wrapper->type();

      // We need to add the version ID back, that was stripped away above. This
      // is violating RESTful principles a little bit.
      if (isset($vuuid)) {
        $entity->vuuid = $vuuid;
        $entity->revision = TRUE;
      }

      $data = drupal_json_encode(array('entity' => $entity));
      $this->setHeader('Content-Type', 'application/json');
      $this->httpPut('uuid_entity', $uuid, $data, 'json');
    }
  }

}
