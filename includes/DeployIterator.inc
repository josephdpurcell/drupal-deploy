<?php

/**
 * Just a wrapper for now.
 */
class DeployIteratorIterator extends RecursiveIteratorIterator {}

/**
 * Iterator which does the heavy lifting.
 *
 * @todo
 *   We need to throw an exception when we detect a circular dependency.
 */
class DeployIterator implements RecursiveIterator {

  var $collection = array();

  var $parent = NULL;

  var $entityType = NULL;

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
      $this->entityType = $key;
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
    $iterator = new DeployIterator($collection);
    $iterator->parent = $key;
    $iterator->entityType = $this->entityType;
    return $iterator;
  }

  function current() {
    $current = current($this->collection);
    if (is_array($current)) {
      return $current;
    }
    // TODO: Figure out how we can parse and make use of the entity type keys
    // in $collection and not hard code to 'node'.
    $entity = reset(entity_load($this->entityType, array($this->key())));
    return entity_metadata_wrapper($this->entityType, $entity);
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
