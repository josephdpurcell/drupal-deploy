<?php

interface DeployProvider {

  function __construct(Array $config = array());

  /**
   * Returns a collection (a structured array) of entities for deployment.
   * Dependencies does not need to be taken into consideration here.
   *
   * The structure of the array that should be returned is documented in the
   * DeployEntityIterator class.
   *
   * @see DeployEntityIterator::__construct()
   */
  function getEntityIterator();

  function configForm(&$form_state);

}
