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
 *
 * @todo
 *   Look into Entity Property API to see if we can do anything cool/effective
 *   with the chaining feature there.
 */
class DeployIterator implements RecursiveIterator {

  var $collection = array();

  var $denendencies = array();

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
    static $cache = array();

    $key = $this->key();
    $current = $this->current();
    if (is_array($current)) {
      $this->entityType = $key;
      return TRUE;
    }

    // Don't check for dependencies twice. They should already have been taken
    // into account.
    //
    // TODO: Or have earlier dependencies really been taken into account? How
    // are they positioned in this recursive mess?
    if (!isset($cache[$this->entityType][$key])) {
      // Just to be clear, at this point $current is a full entity wrapper, loaded
      // through entity_load_by_uuid(). So all the properties are in UUID format.
      $entity_wrapper = $current;
      // But, to save all the implementers from having to reverse-lookup local
      // IDs for simple dependency detection, we load the same entity again
      // through entity_load() so we only have local IDs.
      $entity = reset(entity_load($this->entityType, array($key), NULL, TRUE));
      if (!empty($entity)) {
        $this->dependencies[$key] = module_invoke_all('deploy_entity_dependencies', $entity, $this->entityType);
        // Let other modules have their say.
        drupal_alter('deploy_entity_dependencies', $this->dependencies[$key], $entity, $this->entityType);
      }

      // Now mark this as checked.
      $cache[$this->entityType][$key] = TRUE;

      if (!empty($this->dependencies[$key])) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Gets the children.
   */
  function getChildren() {
    $key = $this->key();
    $current = $this->current();

    $collection = array();
    // Check if we have dependencies directly in the tree in the form of a
    // nested array.
    if (is_array($current)) {
      $collection = $current;
      // Now add the parent as a part of the collection, but only if it seems to
      // be an entity id key and not an entity type key.
      if (is_numeric($key)) {
        $collection[$key] = TRUE;
      }
    }

    if (!empty($this->dependencies[$key])) {
      // TODO: We need to merge with $collection from above in some smart way.
      $collection = $this->dependencies[$key];

      // In an iterator, having children means that the current key it self isn't a
      // part of the collection. However, this isn't the case when doing
      // deployments. So we add the parent as a part of the collection. And since
      // children always should go first, we add the parent last.
      $collection[$this->entityType][$key] = TRUE;

      // TODO: Invoke something like 'hook_deploy_entity_belongings()' that adds
      // tight belongings (reverse dependencies), like comments for nodes, to
      // the collection.
    }

    $iterator = new DeployIterator($collection);
    $iterator->entityType = $this->entityType;
    return $iterator;
  }

  /**
   * Get the current value (entity).
   *
   * @todo Implement static caching to avoid multiple entity loads.
   */
  function current() {
    $current = current($this->collection);
    if (is_array($current)) {
      return $current;
    }
    $uuids = entity_get_uuid_by_id($this->entityType, array($this->key()));
    $entity = reset(entity_uuid_load($this->entityType, $uuids, NULL, TRUE));
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
