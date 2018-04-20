using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GravityOrbiter : MonoBehaviour {

	public float AngCorrectionSpeed;
	public Vector2 RelativeUp { get; private set; }
	private Vector2 GravCounter;

	private GravityCenter PrimaryGravitySource;
	private Vector2 GreatestGravity;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

	void FixedUpdate()
	{
		Vector2 newRelativeUp = -GreatestGravity.normalized;

		RelativeUp = Vector3.Slerp(RelativeUp, newRelativeUp, AngCorrectionSpeed );
			// NOTE Do we want to slerp this with a turn rate instead?

		GreatestGravity = Vector3.zero;
	}

	void OnGravityUpdate(GravityUpdate gravity)
	{
		if(gravity.acceleration.sqrMagnitude > GreatestGravity.sqrMagnitude)
		{
			GreatestGravity = gravity.acceleration;
			PrimaryGravitySource = gravity.source;
		}

		// Debug.DrawRay(transform.position, gravity.acceleration.normalized, Color.cyan, 0.1f);
	}
}
