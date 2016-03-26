<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Main extends CI_Controller {

	/**
	 * Index Page for this controller.
	 *
	 * Maps to the following URL
	 * 		http://example.com/index.php/main
	 *	- or -
	 * 		http://example.com/index.php/main/index
	 *	- or -
	 * Since this controller is set as the default controller in
	 * config/routes.php, it's displayed at http://example.com/
	 *
	 * So any other public methods not prefixed with an underscore will
	 * map to /index.php/welcome/<method_name>
	 * @see https://codeigniter.com/user_guide/general/urls.html
	 */

        public function getCurrentSensors () {
           $view_data = array();
           $view_data['ph_sensor'] = $this->getDataFromTable('ph_sensor');
           $view_data['temp_sensor'] = $this->getDataFromTable('temp_sensor');
           $view_data['uv_sensor'] = $this->getDataFromTable('uv_sensor');
           return $view_data;
        }

        public function getDataFromTable ( $table ) {
            $retval = 0.0;
            $query = $this->db->query("SELECT data FROM $table ORDER BY id DESC LIMIT 1");
            foreach ($query->result() as $row) {
               return $row->data;
            }
            return $retval;

        }


        public function getSensorData ( $table ) {
            $query = $this->db->query("SELECT * FROM $table ORDER BY id DESC LIMIT 100");
            $data = array();
            foreach ($query->result() as $row) {
                $temp = [((strtotime($row->timestamp) + (8 * 3600 ) ) * 1000 ), $row->data];

                $data[] = $temp;
            }

            $retval['data'] = $data;
            $retval['label'] = $table;
            echo json_encode($retval);

        }

	public function index() {
	    $this->load->view('main', $this->getCurrentSensors());
	}

	public function settings() {
            $view_data['delay'] = 0;
            $query = $this->db->query("SELECT data FROM settings WHERE id=1");
            foreach ($query->result() as $row) {
               $view_data['delay'] = $row->data;
            }
	    $this->load->view('settings', $view_data);
	}

	public function savesettings() {
            $delay = $this->input->post('delay');
            $this->db->query("UPDATE settings SET data=$delay WHERE id=1");
            $curl = curl_init('http://localhost:8888/restart');
            curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
            $result = curl_exec($curl);
            $this->index();
	}


}
