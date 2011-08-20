<?php

/**
 * Page callback for the overview page.
 */
function deploy_ui_overview_page() {
  $plans = deploy_plan_load_all();
  $blocks = array();

  // Iterate over all plans.
  foreach ($plans as $plan) {
    $info = array();

    // Get the entity keys from the aggregator.
    $entity_keys = $plan->processor->aggregator->getEntities();
    foreach ($entity_keys as $entity_type => $entity_ids) {
      // Get the entity info and all entities of this type.
      $entity_info = entity_get_info($entity_type);
      $entities = entity_load($entity_type, array_keys($entity_ids));

      // Construct a usable array for the theme function.
      foreach ($entities as $entity) {
        $title = check_plain($entity->{$entity_info['entity keys']['label']});
        if (isset($entity_info['uri callback'])) {
          $uri = $entity_info['uri callback']($entity);
          $title = l($title, $uri['path']);
        }
        $info[] = array(
          'title' => $title,
          'type' => $entity_info['label'],
        );
      }
    }

    // Construct a usable array for the theme function.
    $blocks[] = array(
      'name' => check_plain($plan->name),
      'title' => check_plain($plan->title),
      'description' => check_plain($plan->description),
      'content' => theme('deploy_ui_overview_plan_content', array('info' => $info)),
    );
  }
  return theme('deploy_ui_overview', array('blocks' => $blocks));
}

/**
 * Theme callback for a plan's content.
 */
function theme_deploy_ui_overview_plan_content($variables) {
  $info = $variables['info'];
  $header = array(t('Title'), t('Type'));
  $rows = array();

  foreach ($info as $values) {
    $rows[] = array($values['title'], $values['type']);
  }
  if (empty($rows)) {
    $rows = array(array(array(
      'colspan' => 2,
      'data' => t('No content available.'),
    )));
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Theme callback for the overview page.
 */
function theme_deploy_ui_overview($variables) {
  $blocks = $variables['blocks'];
  $container = array();

  $i = 0;
  foreach ($blocks as $block) {
    $position = ++$i % 2 ? 'left' : 'right';
    $container[$position][] = $block;
  }

  $output = '<div class="admin clearfix">';
  foreach ($container as $position => $blocks) {
    foreach ($blocks as $block) {
      $actions = array(array(
        'title' => t('Deploy'),
        'href' => 'admin/structure/deploy/plans/list/' . $block['name'] . '/deploy',
      ));
      $output .= '<div class="' . $position . ' clearfix">';
      $output .= '<div class="admin-panel">';
      $output .= '<h3>' . $block['title'] . '</h3>';
      $output .= '<div class="description">';
      $output .= $block['description'];
      $output .= '</div>';
      $output .= theme('links', array('links' => $actions, 'attributes' => array('class' => 'action-links')));
      $output .= '<div class="body">';
      $output .= $block['content'];
      $output .= '</div>';
      $output .= '</div>';
      $output .= '</div>';
    }
  }
  $output .= '</div>';
  return $output;
}
