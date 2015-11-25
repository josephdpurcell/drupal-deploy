<?php

/**
 * @file
 * Contains \Drupal\deploy\Plugin\EndpointManager.
 */

namespace Drupal\deploy\Plugin;

use Drupal\Core\Plugin\DefaultPluginManager;
use Drupal\Core\Cache\CacheBackendInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;

/**
 * Provides the Endpoint plugin manager.
 */
class EndpointManager extends DefaultPluginManager {

  /**
   * Constructor for EndpointManager objects.
   *
   * @param \Traversable $namespaces
   *   An object that implements \Traversable which contains the root paths
   *   keyed by the corresponding namespace to look for plugin implementations.
   * @param \Drupal\Core\Cache\CacheBackendInterface $cache_backend
   *   Cache backend instance to use.
   * @param \Drupal\Core\Extension\ModuleHandlerInterface $module_handler
   *   The module handler to invoke the alter hook with.
   */
  public function __construct(\Traversable $namespaces, CacheBackendInterface $cache_backend, ModuleHandlerInterface $module_handler) {
    parent::__construct('Plugin/Endpoint', $namespaces, $module_handler, 'Drupal\deploy\Plugin\EndpointInterface', 'Drupal\deploy\Annotation\Endpoint');

    $this->alterInfo('deploy_endpoint_info');
    $this->setCacheBackend($cache_backend, 'deploy_endpoint_plugins');
  }

}
