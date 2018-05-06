using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlanetGenerator : MonoBehaviour 
{

	public GameObject TurfPrefab;
	public float Radius;

	public float Density;

	public float AngNoise;
	public void Generate()
	{
		if(Density <= 0.0f)
		{
			Debug.LogError("Density must be positive!");
			return;
		}
		for(float x = -Radius;
			x <= Radius;
			x += Density)
			{
				for(float y = -Radius;
					y <= Radius;
					y += Density)
				{
					if(x * x + y * y >= Radius * Radius) continue;
					float angle = Mathf.Rad2Deg * Mathf.Atan2(y, x);
					Vector2 pos = new Vector2(transform.position.x + x, transform.position.y + y);
					CreateTurf(pos, angle);
				}
			}
	}
	private GameObject CreateTurf(Vector2 pos, float angle)
	{
		Quaternion rot = Quaternion.Euler(0, 0, angle + Random.Range(0.0f, AngNoise));
		GameObject turf = GameObject.Instantiate(TurfPrefab, pos, rot, transform);
		
		Rigidbody2D turfBody = turf.GetComponent<Rigidbody2D>();
		if(turfBody)
		{
			turfBody.bodyType = RigidbodyType2D.Static;
		}

		return turf;
	}

	public void CleanUp()
	{
		// Destroy all child objects
		for(int i = 0; i < transform.childCount;)
		{
			Transform child = transform.GetChild(i);
			GameObject.DestroyImmediate(child.gameObject);
		}
	}

	// Use this for initialization
	void Start () 
	{
		CleanUp();
		Generate();
	}

	public void OnDestroy()
	{
		CleanUp();
	}

}
