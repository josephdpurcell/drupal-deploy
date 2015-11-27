<?php

/**
 * @file
 * Contains \Drupal\deploy\Plugin\EndpointBase.
 */

namespace Drupal\deploy\Plugin;

use Drupal\Component\Plugin\PluginBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Base class for Endpoint plugins.
 */
abstract class EndpointBase extends PluginBase implements EndpointInterface {

    /**
     * @inheritDoc
     */
    public function getConfiguration()
    {
        // TODO: Implement getConfiguration() method.
    }

    /**
     * @inheritDoc
     */
    public function setConfiguration(array $configuration)
    {
        // TODO: Implement setConfiguration() method.
    }

    /**
     * @inheritDoc
     */
    public function defaultConfiguration()
    {
        // TODO: Implement defaultConfiguration() method.
    }

    /**
     * @inheritDoc
     */
    public function calculateDependencies()
    {
        // TODO: Implement calculateDependencies() method.
    }

    /**
     * @inheritDoc
     */
    public function buildConfigurationForm(array $form, FormStateInterface $form_state)
    {
        // TODO: Implement buildConfigurationForm() method.
    }

    /**
     * @inheritDoc
     */
    public function validateConfigurationForm(array &$form, FormStateInterface $form_state)
    {
        // TODO: Implement validateConfigurationForm() method.
    }

    /**
     * @inheritDoc
     */
    public function submitConfigurationForm(array &$form, FormStateInterface $form_state)
    {
        // TODO: Implement submitConfigurationForm() method.
    }

    /**
     * @inheritDoc
     */
    public function getScheme()
    {
        // TODO: Implement getScheme() method.
    }

    /**
     * @inheritDoc
     */
    public function getAuthority()
    {
        // TODO: Implement getAuthority() method.
    }

    /**
     * @inheritDoc
     */
    public function getUserInfo()
    {
        // TODO: Implement getUserInfo() method.
    }

    /**
     * @inheritDoc
     */
    public function getHost()
    {
        // TODO: Implement getHost() method.
    }

    /**
     * @inheritDoc
     */
    public function getPort()
    {
        // TODO: Implement getPort() method.
    }

    /**
     * @inheritDoc
     */
    public function getPath()
    {
        // TODO: Implement getPath() method.
    }

    /**
     * @inheritDoc
     */
    public function getQuery()
    {
        // TODO: Implement getQuery() method.
    }

    /**
     * @inheritDoc
     */
    public function getFragment()
    {
        // TODO: Implement getFragment() method.
    }

    /**
     * @inheritDoc
     */
    public function withScheme($scheme)
    {
        // TODO: Implement withScheme() method.
    }

    /**
     * @inheritDoc
     */
    public function withUserInfo($user, $password = null)
    {
        // TODO: Implement withUserInfo() method.
    }

    /**
     * @inheritDoc
     */
    public function withHost($host)
    {
        // TODO: Implement withHost() method.
    }

    /**
     * @inheritDoc
     */
    public function withPort($port)
    {
        // TODO: Implement withPort() method.
    }

    /**
     * @inheritDoc
     */
    public function withPath($path)
    {
        // TODO: Implement withPath() method.
    }

    /**
     * @inheritDoc
     */
    public function withQuery($query)
    {
        // TODO: Implement withQuery() method.
    }

    /**
     * @inheritDoc
     */
    public function withFragment($fragment)
    {
        // TODO: Implement withFragment() method.
    }

    /**
     * @inheritDoc
     */
    public function __toString()
    {
        // TODO: Implement __toString() method.
    }
}
