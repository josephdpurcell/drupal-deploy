<?php

/**
 * @file
 * Deploy Views Entity Link field handler.
 */

/**
 * Class for handling the Deploy Manager Entity Link "field",
 */
class deploy_views_handler_entity_link_field extends deploy_views_handler_base {

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    $entity = $this->entity_load($values);
    $label = entity_label($values->entity_type, $entity);
    $uri = entity_uri($values->entity_type, $entity);


    if (empty($values->revision_id)) {
      $text = t('@entity_label', ['@entity_label' => $label]);
    }
    else {
      $text = t('@entity_label [rev:@rev]', ['@entity_label' => $label, '@rev' => $values->revision_id]);
    }

    $link = l($text, $uri['path'], $uri['options']);
    return $link;
  }
}
