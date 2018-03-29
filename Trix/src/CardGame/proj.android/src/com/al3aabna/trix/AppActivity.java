package com.al3aabna.trix;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.content.Intent;
import android.os.Bundle;

import com.al3aabna.trix.fb.FacebookConnect;
import com.al3aabna.trix.fb.FacebookFriend;
import com.al3aabna.trix.fb.FacebookPost;
import com.al3aabna.trix.fb.FacebookScore;
import com.al3aabna.trix.inapp.GoogleBilling;

public class AppActivity extends Cocos2dxActivity {

	private GoogleBilling gBilling = null;
	private GoogleAdvertise gAdvertise = null;
	private FacebookConnect fbConnect = null;
	private FacebookFriend fbFriend = null;
	private FacebookPost fbPost = null;
	private FacebookScore fbScore = null;
	private AppService appService = null;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// in app billing
		gBilling = new GoogleBilling(this);
		gBilling.startService();

		// advertisement
		gAdvertise = new GoogleAdvertise(this);
		gAdvertise.onCreate();

		// facebook
		fbConnect = new FacebookConnect(this);
		fbFriend = new FacebookFriend(this);
		fbPost = new FacebookPost(this);
		fbScore = new FacebookScore(this);

		fbConnect.onCreate(savedInstanceState);
		fbFriend.onCreate(savedInstanceState);
		fbPost.onCreate(savedInstanceState);
		fbScore.onCreate(savedInstanceState);

		appService = AppService.init(this);
		appService.onCreate();
	}

	@Override
	public void onResume() {
		super.onResume();

		fbConnect.onResume();
		fbFriend.onResume();
		fbPost.onResume();
		fbScore.onResume();
	}

	@Override
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);

		gBilling.onActivityResult(requestCode, resultCode, data);
		fbConnect.onActivityResult(requestCode, resultCode, data);
		fbFriend.onActivityResult(requestCode, resultCode, data);
		fbPost.onActivityResult(requestCode, resultCode, data);
		fbScore.onActivityResult(requestCode, resultCode, data);
	}

	@Override
	public void onPause() {
		super.onPause();

		fbConnect.onPause();
		fbFriend.onPause();
		fbPost.onPause();
		fbScore.onPause();
	}

	@Override
	public void onDestroy() {
		super.onDestroy();

		gBilling.stopService();
		gAdvertise.onDestroy();
		fbConnect.onDestroy();
		fbFriend.onDestroy();
		fbPost.onDestroy();
		fbScore.onDestroy();
		appService.onDestroy();

		gBilling.release();
		gAdvertise.release();
		fbConnect.release();
		fbFriend.release();
		fbPost.release();
		fbScore.release();
		appService.release();
	}

	@Override
	public void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);

		fbConnect.onSaveInstanceState(outState);
		fbFriend.onSaveInstanceState(outState);
		fbPost.onSaveInstanceState(outState);
		fbScore.onSaveInstanceState(outState);
	}

	@Override
	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = super.onCreateView();
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
		return glSurfaceView;
	}

}
