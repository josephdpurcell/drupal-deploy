<?php

class DeployServiceRestJSON extends DeployServiceRest {

  /**
   * @todo: DeployService::deploy() will take DeployEntityIterator as parameter.
   */
  function deploy(Array $entity_ids) {
    // @todo: Almost all of this logic will go into DeployEntityIterator.
    foreach ($entity_ids as $entity_type => $ids) {
      $entities = entity_load($entity_type, $ids);
      foreach ($entities as $id => $entity) {
        // We need the property info given to us by the Entity API module.
        $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);
        $bundle = $entity_wrapper->getBundle();

        // To follow REST principles strictly we can only deploy entities with
        // properties that are writable. So we need to go through all of them.
        foreach ($entity as $key => $value) {
          $property_info = $entity_wrapper->getPropertyInfo();
          if (!isset($property_info[$key]) || !isset($property_info[$key]['setter callback'])) {
            unset($entity->{$key});
          }
        }

        $data = drupal_json_encode($entity);
        $this->httpPost($entity_type, $id, $data, 'json');
      }
    }
  }

}
