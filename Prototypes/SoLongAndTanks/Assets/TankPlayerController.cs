using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(CircleCollider2D))]
class TankPlayerController : MonoBehaviour 
{

	private CircleCollider2D Collider;

	public float TreadSpeed;
	public float AngCorrectionSpeed;
	public bool IsGrounded {get; private set;}
	private Vector2 GroundNormal;

	// Use this for initialization
	void Start () {
		Collider = GetComponent<CircleCollider2D>();
	}
	
	void OnCollisionEnter2D(Collision2D col)
	{
		IsGrounded = true;

		// Ground normal is average of all contact normals I guess
		GroundNormal *= 0.0f;
		foreach(var contact in col.contacts)
		{
			GroundNormal += contact.normal;
		}
		GroundNormal.Normalize();

	}

	// Update is called once per frame
	void Update () 
	{
		Debug.DrawRay(transform.position, GroundNormal, Color.red, 0.1f);
	
		if(!IsGrounded)
		{
			return;
		}

		float targetAngDelt = Vector3.Angle(transform.up, GroundNormal);
		float angDelt = Mathf.Max(targetAngDelt, AngCorrectionSpeed * Time.deltaTime);

		transform.Rotate(Vector3.forward, angDelt);

		if(Input.GetAxis("Horizontal") > 0.1f)
		{
			Vector3 moveForce = TreadSpeed * transform.right;
			GetComponent<Rigidbody2D>().AddForce(moveForce, ForceMode2D.Force);
		}
		else if(Input.GetAxis("Horizontal") < -0.1f)
		{
			Vector3 moveForce = -TreadSpeed * transform.right;
			GetComponent<Rigidbody2D>().AddForce(moveForce, ForceMode2D.Force);
		}
	
	}


}
