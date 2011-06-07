<?php

/**
 * This iterator class takes a structured array of entity ids and their entity
 * types, iterates over them, finds all their dependencies and returns full
 * entity objects in a deployable order.
 *
 * @todo
 *   We need to throw an exception when we detect a circular dependency.
 */
class DeployEntityIterator implements RecursiveIterator {

  var $collection = array();

  var $parent = NULL;

  /**
   * Constructor.
   *
   * The $collection array should be structured as below, where 'foo', 'bar' and
   * 'baz' are entity types and the numeric keys are entity ids. Although it's
   * not required, the value of each key can be a similar structured array of
   * dependencies that will be parsed recursively.
   * @code
   *   $collection = array(
   *     'foo' => array(
   *       10 => TRUE,
   *       12 => array(
   *         'baz' => array(
   *           14 => TRUE,
   *         ),
   *       ),
   *     ),
   *     'bar' => array(
   *       14 => TRUE,
   *     ),
   *   );
   * @endcode
   *
   * @param $collection
   *   A structured array of entity ids and their entity types.
   */
  function __construct($collection) {
    $this->collection = $collection;
  }

  function hasChildren() {
    $key = $this->key();
    $current = $this->current();
    if (is_array($current)) {
      return TRUE;
    }
    // TODO: Invoke hook that returns all references to this key.
    $references = array();
    // Probably an entity id key, and if it is the parent, it doesn't have any
    // more children than those we are already parsing in this collection.
    if (isset($references[$key]) && $key != $this->parent) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Gets the children.
   *
   * In an iterator, having children means that the current key it self isn't a
   * part of the collection. However, this isn't the case when doing
   * deployments. So we add the parent as a part of the collection. And since
   * children always should go first, we add the parent last.
   */
  function getChildren() {
    $key = $this->key();
    $current = $this->current();

    $parent = NULL;
    $collection = array();
    // Check if we have dependencies directly in the tree in the form of a
    // nested array.
    if (is_array($current)) {
      $collection = $current;
      // Now add the parent as a part of the collection, but only if it seems to
      // be an entity id key and not an entity type key.
      if (is_numeric($key)) {
        $collection[$key] = TRUE;
        $parent = $key;
      }
    }
    // Check if other modules thinks that this one has some dependencies.
    // TODO: Invoke hook that returns all references to this key.
    $references = array();
    if (isset($references[$key])) {
      // We must merge recursive with what we might got above.
      // TODO: We need to merge with $collection from above in some smart way.
      $collection = $references[$key];
      // Add the parent as a part of the collection.
      $collection[$key] = TRUE;
      $parent = $key;
    }
    $iterator = new DeployEntityIterator($collection);
    $iterator->parent = $key;
    return $iterator;
  }

  function current() {
    return current($this->collection);
  }

  function key() {
    return key($this->collection);
  }

  function next() {
    return next($this->collection);
  }

  function rewind() {
    reset($this->collection);
  }

  function valid() {
    $key = key($this->collection);
    return ($key !== NULL && $key !== FALSE);
  }

}

/**
 * Just a wrapper for now.
 */
class DeployEntityIteratorIterator extends RecursiveIteratorIterator {}
