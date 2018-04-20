using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(CircleCollider2D))]
public class GravityCenter : MonoBehaviour 
{

	public float magnitude = 10.0f;
	public AnimationCurve falloff;

	private CircleCollider2D collider;

	// Use this for initialization
	void Start () {
		collider = GetComponent<CircleCollider2D>();
		collider.isTrigger = true;
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	void OnTriggerStay2D(Collider2D other)
	{
		Vector3 grav = (transform.position - other.transform.position);
		float dist = grav.magnitude / collider.radius; // should be [0..1]

		grav = grav.normalized * falloff.Evaluate(dist) * magnitude;

		other.GetComponent<Rigidbody2D>().AddForce(grav, ForceMode2D.Force);

		other.SendMessage("OnGravityUpdate", new GravityUpdate(grav,this), 
			SendMessageOptions.DontRequireReceiver);
	}

}

public struct GravityUpdate
{
	public GravityUpdate(Vector3 grav, GravityCenter src)
	{
		acceleration = grav;
		source = src;
	}
	public Vector2 acceleration;
	public GravityCenter source;
}