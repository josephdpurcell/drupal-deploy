<?php

/**
 * @file
 * Contains \Drupal\deploy\Plugin\EndpointInterface.
 */

namespace Drupal\deploy\Plugin;

use Drupal\Component\Plugin\PluginInspectionInterface;
use Psr\Http\Message\UriInterface;
/**
 * Defines an interface for Endpoint plugins.
 */
interface EndpointInterface extends PluginInspectionInterface, UriInterface  {

  // Add get/set methods for your plugin type here.

}
