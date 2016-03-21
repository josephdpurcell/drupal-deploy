<?php
/**
 * @file
 * Deploy UI page functions.
 */

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
      'data' => t('Currently no content in this plan.'),
    )));
  }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Theme callback for the plan view page.
 */
function theme_deploy_ui_plan_view($variables) {
  drupal_add_css(drupal_get_path('module', 'deploy_ui') . '/css/deploy_ui.admin.css');

  $data = $variables['data'];

  return '<div class="description">'
    . '<h4 class="element-invisible">Plan description</h4>'
    . $data['plan_description']
    . '</div>'
    . '<div class="body">'
    . $data['content']
    . '</div>';
}

/**
 * Form constructor for the empty plan page.
 */
function deploy_ui_empty_plan_form($form, &$form_state, $plan) {
  $form = array();

  $form['deploy_plan'] = array(
    '#type' => 'value',
    '#value' => $plan,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to empty the @plan plan?', array('@plan' => $plan->title)),
    'admin/structure/deploy/plans/list/' . $plan->name,
    t('Emptying the plan will remove all items from the plan.'),
    t('Empty'),
    t('Cancel')
  );
}

/**
 * Submit handler for empty plan form.
 */
function deploy_ui_empty_plan_form_submit($form, &$form_state) {
  $plan = $form_state['values']['deploy_plan'];
  $plan->emptyPlan();

  drupal_set_message(
    t('All items were removed from the @plan plan.', array('@plan' => $plan->title))
  );

  $form_state['redirect'] = 'admin/structure/deploy/plans/list/' . $plan->name;
}

/**
 * Callback for the flatten plan page.
 */
function deploy_ui_flatten_plan_form($form, &$form_state, $plan) {
  $form = array();

  $form['deploy_plan'] = array(
    '#type' => 'value',
    '#value' => $plan,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to flatten the @plan plan?', array('@plan' => $plan->title)),
    'admin/structure/deploy/plans/list/' . $plan->name,
    t('Flattening the plan will remove all duplicated items from the plan.'),
    t('Flatten'),
    t('Cancel')
  );
}

/**
 * Submit handler for empty plan form.
 */
function deploy_ui_flatten_plan_form_submit($form, &$form_state) {
  $plan = $form_state['values']['deploy_plan'];
  $plan->flatten();

  drupal_set_message(
    t('The @plan plan was flattened.', array('@plan' => $plan->title))
  );

  $form_state['redirect'] = 'admin/structure/deploy/plans/list/' . $plan->name;
}
