<?php

namespace Drupal\deploy;

interface DeployInterface {

  public function push($target_domain, $target_workspace);

}