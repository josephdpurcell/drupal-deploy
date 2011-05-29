<?php

$plugin = array(
  'schema' => 'deploy_setups',
  'access' => 'administer deployments',

  'menu' => array(
    'menu prefix' => 'admin/structure/deploy',
    'menu item' => 'setups',
    'menu title' => 'Setups',
  ),

  'title singular' => t('setup'),
  'title singular proper' => t('Setup'),
  'title plural' => t('setups'),
  'title plural proper' => t('Setups'),

  'use wizard' => TRUE,

  'handler' => array(
    'class' => 'deploy_ui_setup',
    'parent' => 'ctools_export_ui',
  ),

  'form info' => array(
    'order' => array(
      'basic' => t('Basics'),
      'fetcher' => t('Fetcher'),
      'worker' => t('Worker'),
      'endpoint' => t('Endpoints'),
    ),
  ),
);
