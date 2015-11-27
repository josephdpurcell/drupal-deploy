<?php

/**
 * @file
 * Contains \Drupal\deploy\EndpointListBuilder.
 */

namespace Drupal\deploy;

use Drupal\Core\Config\Entity\ConfigEntityListBuilder;
use Drupal\Core\Entity\EntityInterface;

/**
 * Provides a listing of Endpoint entities.
 */
class EndpointListBuilder extends ConfigEntityListBuilder {
  /**
   * {@inheritdoc}
   */
  public function buildHeader() {
    $header['label'] = $this->t('Endpoint');
    $header['id'] = $this->t('Machine name');
    return $header + parent::buildHeader();
  }

  /**
   * {@inheritdoc}
   */
  public function buildRow(EntityInterface $entity) {
    $row['label'] = $entity->label();
    $row['id'] = $entity->id();
    // You probably want a few more properties here...
    return $row + parent::buildRow($entity);
  }

  /**
   * {@inheritdoc}
   */
  public function render() {
    $build['table'] = parent::render();
    $build['form'] = \Drupal::formBuilder()->getForm('Drupal\deploy\Form\EndpointSetupForm');
    return $build;
  }

}
