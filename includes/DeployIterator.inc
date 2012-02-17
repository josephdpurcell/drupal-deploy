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
    return new DeployIterator($this->getChildrenEntities(), $this);
  }

  /**
   * We override this method since we need to return UUID entities, with some
   * extra hooks.
   *
   * @see http://www.odata.org/developers/protocols
   */
  public function current() {
    $uuids = entity_get_uuid_by_id($this->entityType, array($this->entityId));
    $uuid = reset($uuids);
    // Load the current entity with UUID. We need to reset the cache here in
    // order to ensure that we are deploying a fresh version if the entity.
    $entities = entity_uuid_load($this->entityType, array($uuid), NULL, TRUE);
    $entity = reset($entities);
    // Add necessary metadata to the entity.
    $cause = FALSE;
    if (!empty($this->causes[$this->entityType][$this->entityId])) {
      $cause_type = $this->causes[$this->entityType][$this->entityId]['type'];
      $cause_uuids = entity_get_uuid_by_id($cause_type, array($this->causes[$this->entityType][$this->entityId]['id']));
      $cause_uuid = reset($cause_uuids);
      $cause = $cause_type . '/' . $cause_uuid;
    }
    $entity->__metadata = array(
      'type' => $this->entityType,
      'uri' => $this->entityType . '/' . $uuid,
      'cause' => $cause,
    );
    // Now mark this as traversed.
    $this->traversed[$this->entityType][$this->entityId] = TRUE;
    // Let other modules have their say.
    drupal_alter('deploy_entity', $entity, $this->entityType);
    return $entity;
  }
}
