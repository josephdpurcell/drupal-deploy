<?php

/**
 * @file
 * Deploy module views integration functions.
 */

/**
 * Implements hook_views_data().
 */
function deploy_views_data() {
  $data = ['deploy_manager_entities' => []];

  $data['deploy_manager_entities'] = [
    'table' => [
      'group' => t('Deploy'),
        // We can't do dymanic joins to the various entity tables. More on this later.
      'join' => [],
      'base' => [
        'title' => t('Deploy Manager Entities'),
        'help' => t("Table containing the entities from deploy's managed plans."),
      ],
    ],
    'plan_name' => [
      'title' => t('Plan name'),
      'help' => t('The name of the deployment plan.'),
      'field' => [
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ],
      'sort' => [
        'handler' => 'views_handler_sort',
      ],
      'filter' => [
        'handler' => 'views_handler_filter_string',
      ],
      'argument' => [
        'handler' => 'views_handler_argument_string',
      ],
    ],
    'entity_type' => [
      'title' => t('Entity Type'),
      'help' => t('The machine name of the entity type.'),
      'field' => [
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ],
      'sort' => [
        'handler' => 'views_handler_sort',
      ],
      'filter' => [
        'handler' => 'views_handler_filter_string',
      ],
      'argument' => [
        'handler' => 'views_handler_argument_string',
      ],
    ],
    'entity_id' => [
      'title' => t('Entity ID'),
      'help' => t('The id of the entity.'),
      'field' => [
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ],
      'filter' => [
        'handler' => 'views_handler_filter_numeric',
      ],
      'sort' => [
        'handler' => 'views_handler_sort',
      ],
    ],
    'revision_id' => [
      'title' => t('Revision ID'),
      'help' => t('The id of the entity revision.'),
      'field' => [
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ],
      'filter' => [
        'handler' => 'views_handler_filter_numeric',
      ],
      'sort' => [
        'handler' => 'views_handler_sort',
      ],
    ],
    'timestamp' => [
      'title' => t('Timestamp'),
      'help' => t('Time when entity was added to the plan.'),
      'field' => [
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ],
      'filter' => [
        'handler' => 'views_handler_sort_date',
      ],
      'sort' => [
        'handler' => 'views_handler_filter_date',
      ],
    ],
    // Custom fields
    'entity_label' => [
      'title' => t('Entity Label'),
      'help' => t('The label of the entity.'),
      'field' => [
        'handler' => 'deploy_views_handler_entity_label_field',
      ],
    ],
    'entity_link' => [
      'title' => t('Entity Link'),
      'help' => t('The link to the entity.'),
      'field' => [
        'handler' => 'deploy_views_handler_entity_link_field',
      ],
    ],
    'entity_type_label' => [
      'title' => t('Entity Type Label'),
      'help' => t('The label for the entity type.'),
      'field' => [
        'handler' => 'deploy_views_handler_entity_type_label_field',
      ],
    ],
     'related_plans' => [
      'title' => t('Related Plans'),
      'help' => t('Other plans that contain this entity.'),
      'field' => [
        'handler' => 'deploy_views_handler_related_plans_field',
      ],
    ],
  ];

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function deploy_views_handlers() {
  return [
    'info' => [
      'path' => drupal_get_path('module', 'deploy') . '/includes/views',
    ],/*
    'handers' => [
      'deploy_views_handler_entity_label_field' => [
        'parent' => 'deploy_views_handler_base',
      ],
    ],
     */
  ];
}
