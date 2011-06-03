<?php

$plugin = array(
  'schema' => 'deploy_plans',
  'access' => 'administer deployments',

  'menu' => array(
    'menu prefix' => 'admin/structure/deploy',
    'menu item' => 'plans',
    'menu title' => 'Plans',
  ),

  'title singular' => t('plan'),
  'title singular proper' => t('Plan'),
  'title plural' => t('plans'),
  'title plural proper' => t('Plans'),

  'use wizard' => TRUE,

  'handler' => array(
    'class' => 'deploy_ui_plan',
    'parent' => 'ctools_export_ui',
  ),

  'form info' => array(
    'order' => array(
      'basic' => t('Basics'),
      'provider' => t('Provider'),
      'processor' => t('Processor'),
    ),
  ),
);
