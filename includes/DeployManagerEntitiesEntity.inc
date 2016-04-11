<?php

/**
 * @file
 * Developer Manager Entities Entity.
 */

/**
 * Developer Manager Entities Entity class.
 */
class DeployManagerEntitiesEntity extends Entity {

  /**
   * {@inheritdoc}
   */
  public function label() {
    $label = deploy_plan_entity_label($this->entity_type, $this->entity_id, $this->revision_id);
    return $label;
  }
}
