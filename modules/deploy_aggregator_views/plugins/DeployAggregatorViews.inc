<?php

class DeployAggregatorViews implements DeployAggregator {

  var $config = array();

  function __construct(Array $config = array()) {
    $defaults = array(
      'view_name' => '',
    );
    $this->config = array_merge($defaults, $config);
  }

  /**
   * @return DeployIteratorIterator
   *
   * @see DeployAggregator::getIterator()
   */
  function getIterator() {
    $view = views_get_view($this->config['view_name'], 'default');
    // TODO: Consider adding a config parameter for $entity_type, since that's
    // not always consistent with $view->base_table.
    $entity_type = $view->base_table;
    $entity_id_field = $view->base_field;

    $view_results = views_get_view_result($view->name, 'default');
    $entity_ids = array();
    foreach ($view_results as $view_result) {
      $entity_ids[$view_result->{$entity_id_field}] = TRUE;
    }

    $iterator = new DeployIterator(array($entity_type => $entity_ids));
    return new DeployIteratorIterator($iterator);
  }

  function configForm(&$form_state) {
    $form = array();
    $options = array();
    $views = views_get_all_views();

    foreach ($views as $view_name => $view) {
      $options[$view_name] = !empty($view->human_name) ? $view->human_name : $view->name;
    }

    $form['view_name'] = array(
      '#type' => 'select',
      '#title' => t('View'),
      '#description' => t('Select the view that is going to provide this deployment plan with content.'),
      '#options' => $options,
      '#default_value' => $this->config['view_name'],
    );

    return $form;
  }

}
