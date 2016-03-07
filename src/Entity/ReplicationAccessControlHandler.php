<?php

/**
 * @file
 * Contains \Drupal\deploy\Entity\ReplicationAccessControlHandler.
 */

namespace Drupal\deploy\Entity;

use Drupal\Core\Entity\EntityAccessControlHandler;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Access\AccessResult;

/**
 * Access controller for the Replication entity.
 *
 * @see \Drupal\deploy\Entity\Replication.
 */
class ReplicationAccessControlHandler extends EntityAccessControlHandler {
  /**
   * {@inheritdoc}
   */
  protected function checkAccess(EntityInterface $entity, $operation, AccountInterface $account) {

    switch ($operation) {
      case 'view':
        return AccessResult::allowedIfHasPermission($account, 'view replication entities');

      case 'update':
        return AccessResult::allowedIfHasPermission($account, 'edit replication entities');

      case 'delete':
        return AccessResult::allowedIfHasPermission($account, 'delete replication entities');
    }

    return AccessResult::allowed();
  }

  /**
   * {@inheritdoc}
   */
  protected function checkCreateAccess(AccountInterface $account, array $context, $entity_bundle = NULL) {
    return AccessResult::allowedIfHasPermission($account, 'add replication entities');
  }

}
