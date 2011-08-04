<?php

interface DeployAggregator extends IteratorAggregate {

  function __construct(DeployPlan $plan = NULL, Array $config = array());

  /**
   * Returns an iterator with entities for deployment.
   *
   * Since we are dealing with resursive iteration (entities with references),
   * we need to run DeployIterator through DeployIteratorIterator and return the
   * later one.
   *
   * @return DeployIteratorIterator
   *
   * @see http://giorgiosironi.blogspot.com/2010/02/practical-php-patterns-iterator.html
   */
  //function getIterator();

  function configForm(&$form_state);

}
