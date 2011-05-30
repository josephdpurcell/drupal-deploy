<?php

class DeployProviderViews implements DeployProvider {

  var $config = array();

  function __construct(Array $config = array()) {
    $defaults = array(
      'view_name' => '',
    );
    $this->config = array_merge($defaults, $config);
  }

  function getResources() {
    // @todo: Render view, load all entities and return them.
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
