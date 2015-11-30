<?php

/**
 * @file
 * Contains \Drupal\deploy\Entity\Endpoint.
 */

namespace Drupal\deploy\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBase;
use Drupal\deploy\EndpointInterface;
use Drupal\Core\Entity\EntityWithPluginCollectionInterface;
use Drupal\deploy\EndpointPluginCollection;

/**
 * Defines the Endpoint entity.
 *
 * @ConfigEntityType(
 *   id = "endpoint",
 *   label = @Translation("Endpoint"),
 *   handlers = {
 *     "list_builder" = "Drupal\deploy\EndpointListBuilder",
 *     "form" = {
 *       "add" = "Drupal\deploy\Form\EndpointAddForm",
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
 *   config_export = {
 *     "id",
 *     "label",
 *     "uuid",
 *     "plugin",
 *     "configuration",
 *   },
 *   links = {
 *     "canonical" = "/admin/structure/endpoint/{endpoint}",
 *     "edit-form" = "/admin/structure/endpoint/{endpoint}/edit",
 *     "delete-form" = "/admin/structure/endpoint/{endpoint}/delete",
 *     "collection" = "/admin/structure/visibility_group"
 *   }
 * )
 */
class Endpoint extends ConfigEntityBase implements EndpointInterface, EntityWithPluginCollectionInterface {
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

  /**
   * @var
   */
  protected $uuid;

  /**
   * @var
   */
  protected $plugin;

  /**
   * @var array
   */
  protected $configuration = [];

  /**
   * @var
   */
  protected $pluginCollection;

  /**
   * Encapsulates the creation of the endpoint's LazyPluginCollection.
   *
   * @return \Drupal\Component\Plugin\LazyPluginCollection
   *   The endpoint's plugin collection.
   */
  protected function getPluginCollection() {
    if (!$this->pluginCollection) {
      $this->pluginCollection = new EndpointPluginCollection(\Drupal::service('plugin.manager.endpoint.processor'), $this->plugin, $this->configuration);
    }
    return $this->pluginCollection;
  }

  /**
   * @inheritDoc
   */
  public function getPluginCollections()
  {
    return ['configuration' => $this->getPluginCollection()];
  }

  /**
   * @param $plugin_id
   */
  public function setPlugin($plugin_id) {
    $this->plugin = $plugin_id;
    $this->getPluginCollection()->addInstanceId($plugin_id);
  }

  public function getPlugin() {
    return $this->getPluginCollection()->get($this->plugin);
  }
}
