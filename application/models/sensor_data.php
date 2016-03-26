<?php
class sensor_data extends CI_Model {
	
    
    var $caller ;
    var $table_name = 'ph_level';
    	
    var $id;
    var $data;
    var $timestamp;
   
    function __construct() {
        // Call the Model constructor
        parent::__construct();
        $this->caller =& get_instance();
    }
  
    function get_data () {
        $data = array(
            'data' => $this->data,
            'timestamp' => $this->timestamp,
        );
        return $data;
    }
   
    function add ( ) {
         // database insert
         $this->caller->db->insert($this->table_name, $this->get_data());
         // get the id from the last insert
         $this->id  = $this->caller->db->insert_id();
         return $this->id;		 
    }
    
    function update ( ) {
        $this->caller->db->where('id', $this->id);
        // database update
		$this->caller->db->update($this->table_name, $this->get_data());    	
	}
    
	function get ( ) {
		$query = $this->caller->db->query("SELECT * FROM $this->table_name WHERE id='$this->id' LIMIT 1");
   		foreach ($query->result() as $row) {
			$this->id = $row->id;
			$this->data = $row->data;
			$this->timestamp = $row->timestamp;
		}
	}
    
	function add_thru_post ( ) {
		// get the information first and update the model
		$this->data = $this->caller->input->post('data');
		$id = $this->add();
		return $id;
	}
		
	function update_thru_post () {
		// get the information first and update the model
		$this->id = $this->caller->input->post('id');
		$this->data = $this->caller->input->post('data');
		$this->update();
	}
	
	function delete_thru_post () {
		// get the information first and update the model
		$this->id = $this->caller->input->post('id');
		$this->delete();
	}
  function getAll ( $table ) {
    $query = $this->caller->db->query("SELECT * FROM $table LIMIT 100 SORT BY id DESC");
    $total = $this->caller->db->affected_rows();
    $data = array();
    foreach ($query->result() as $row) {
      $data[] = $row;
    }
    return $data;
  }
}
?>
