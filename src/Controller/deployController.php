<?php
/**
 * @file
 * Contains \Drupal\deploy\Controller\deployController.
 */
namespace Drupal\deploy\Controller;
class deployController {
  public function admin() {
    return array(
        '#type' => 'markup',
        '#markup' => t('Insert deploy admin here.'),
    );
  }
}
