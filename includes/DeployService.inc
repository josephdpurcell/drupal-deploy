<?php

interface DeployService {

  function __construct(DeployPlan $plan, Array $config = array());

  function deploy(Traversable $iterator);

  function configForm(&$form_state);

}

interface EntityDeployer {
  function deployEntity($entity_wrapper);
}