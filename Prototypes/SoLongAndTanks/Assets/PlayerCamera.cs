using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerCamera : MonoBehaviour {

	public GameObject player;


	public float AngularSpeed;

	public float MoveSpeed;
	public float SmoothTime;
	private Vector3 originalOffset;
	private Vector3 cameraVelocity = new Vector3();

	// Use this for initialization
	void Start () {
		if(player == null)
		{
			return;
		}

		originalOffset = transform.position - player.transform.position;
	}
	
	// Update is called once per frame
	void Update () {
		if(player == null)
		{
			return;
		}

		float targetAngDelt = Vector3.Angle(transform.up, player.transform.up);
		float angDelt = Mathf.Min(targetAngDelt, AngularSpeed * Time.deltaTime);
		transform.Rotate(Vector3.forward, angDelt);

		Vector3 targetPos = player.transform.position + originalOffset;
		transform.position = Vector3.SmoothDamp(transform.position, targetPos, ref cameraVelocity, SmoothTime, MoveSpeed );

	}
}
