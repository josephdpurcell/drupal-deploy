<?php

/**
 * Page callback for the overview page.
 */
function deploy_ui_overview_page() {
  $blocks = array();
  $plans = deploy_plan_load_all();
  foreach ($plans as $plan) {
    $blocks[] = array(
      'name' => $plan->name,
      'title' => check_plain($plan->title),
      'description' => check_plain($plan->description),
      'content' => theme('deploy_ui_overview_plan_content', array('plan' => $plan)),
    );
  }
  return theme('deploy_ui_overview', array('blocks' => $blocks));
}

/**
 * Theme callback for a plan's content.
 */
function theme_deploy_ui_overview_plan_content($variables) {
  $plan = $variables['plan'];
  $rows = array();
  $header = array(
    t('Title'),
    t('Type'),
  );
  $entities = $plan->processor->aggregator->getEntities();
  foreach ($entities as $entity_type => $entity_ids) {
    $entities = entity_load($entity_type, array_keys($entity_ids));
    foreach ($entities as $entity) {
      $entity_wrapper = entity_metadata_wrapper($entity_type, $entity);
      $rows[] = array(
        $entity_wrapper->label(),
        $entity_wrapper->type(),
      );
    }
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
  $container = array();
  $blocks = $variables['blocks'];
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
