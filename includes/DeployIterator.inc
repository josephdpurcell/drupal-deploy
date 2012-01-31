<?php

/**
 * Iterator class which does the heavy lifting for detecting dependencies.
 */
class DeployIterator extends EntityDependencyIterator {

  /**
   * We override this method since we need to return our own iterator.
   *
   * @return DeployIterator
   */
  public function getChildren() {
    $iterator = new DeployIterator($this->getChildrenEntities(), $this);
    $iterator->entityType = $this->entityType;
    return $iterator;
  }

  /**
   * We override this method since we need to return UUID entities, with some
   * extra hooks.
   *
   * @see http://www.odata.org/developers/protocols
   */
  public function current() {
    $current = current($this->entities);
    if (is_array($current)) {
      return $current;
    }
    $uuids = entity_get_uuid_by_id($this->entityType, array($this->entityId));
    $uuid = reset($uuids);
    // Load the current entity with UUID.
    $entities = entity_uuid_load($this->entityType, array($uuid));
    $entity = reset($entities);
    // Add necessary metadata to the entity.
    $origin = FALSE;
    if (!empty($this->origins[$this->entityType][$this->entityId])) {
      $origin_type = $this->origins[$this->entityType][$this->entityId]['type'];
      $origin_id = $this->origins[$this->entityType][$this->entityId]['id'];
      $origin_uuids = entity_get_uuid_by_id($origin_type, array($origin_id));
      $origin_uuid = reset($origin_uuids);
      $origin = array(
        '__deferred' => array(
          'uri' => $origin_type . '/' . $origin_uuid,
        ),
      );
    }
    $entity->__metadata = array(
      'type' => $this->entityType,
      'uri' => $this->entityType . '/' . $uuid,
      'origin' => $origin,
    );
    // Now mark this as traversed.
    $this->traversed[$this->entityType][$this->entityId] = TRUE;
    // Let other modules have their say.
    drupal_alter('deploy_entity', $entity, $this->entityType);
    return $entity;
  }
}
