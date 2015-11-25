<?php

/**
 * @file
 * Contains \Drupal\deploy\Entity\Endpoint.
 */

namespace Drupal\deploy\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBase;
use Drupal\deploy\EndpointInterface;

/**
 * Defines the Endpoint entity.
 *
 * @ConfigEntityType(
 *   id = "endpoint",
 *   label = @Translation("Endpoint"),
 *   handlers = {
 *     "list_builder" = "Drupal\deploy\EndpointListBuilder",
 *     "form" = {
 *       "add" = "Drupal\deploy\Form\EndpointForm",
 *       "edit" = "Drupal\deploy\Form\EndpointForm",
 *       "delete" = "Drupal\deploy\Form\EndpointDeleteForm"
 *     }
 *   },
 *   config_prefix = "endpoint",
 *   admin_permission = "administer site configuration",
 *   entity_keys = {
 *     "id" = "id",
 *     "label" = "label",
 *     "uuid" = "uuid"
 *   },
 *   links = {
 *     "canonical" = "/admin/structure/endpoint/{endpoint}",
 *     "edit-form" = "/admin/structure/endpoint/{endpoint}/edit",
 *     "delete-form" = "/admin/structure/endpoint/{endpoint}/delete",
 *     "collection" = "/admin/structure/visibility_group"
 *   }
 * )
 */
class Endpoint extends ConfigEntityBase implements EndpointInterface {
  /**
   * The Endpoint ID.
   *
   * @var string
   */
  protected $id;

  /**
   * The Endpoint label.
   *
   * @var string
   */
  protected $label;

}
