package com.al3aabna.tarneeb.fb;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

import com.facebook.FacebookException;
import com.facebook.FacebookOperationCanceledException;
import com.facebook.Session;
import com.facebook.SessionState;
import com.facebook.UiLifecycleHelper;
import com.facebook.widget.FacebookDialog;
import com.facebook.widget.FacebookDialog.PendingCall;
import com.facebook.widget.WebDialog;
import com.facebook.widget.WebDialog.OnCompleteListener;

public class FacebookPost {
	private static final String TAG = "FacebookPost";
	private static final int MSG_POST = 1;

	private static FacebookPost instance;
	private UiLifecycleHelper uiHelper;
	private Session.StatusCallback statusCallback = new SessionStatusCallback();
	private Activity activity;

	static public String name = null;
	static public String caption = null;
	static public String description = null;
	static public String link = null;
	static public String picture = null;

	private static final Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch(msg.what) {
			case MSG_POST:
				instance.postStatus();
				break;
			default:
				break;
			}
		}
	};

	public FacebookPost(Activity activity) {
		this.activity = activity;
		instance = this;
		uiHelper = new UiLifecycleHelper(activity, statusCallback);
	}

	public void release() {
		instance = null;
	}

	public static void nativePostStatus() {
		name = FacebookConnect.nativeGetUserName();
		caption = "Spades Game";
		description = "Spades game is very interesting. Please play this game";
		link = "https://play.google.com/store/apps/details?id=com.spade.spadegame";
		picture = "https://play.google.com/store/apps/details?id=com.spade.spadegame";

		Message message = Message.obtain();
		message.what = MSG_POST;
		mHandler.sendMessage(message);
	}

	private void postStatus() {
		if (FacebookDialog.canPresentShareDialog(activity.getApplicationContext(), FacebookDialog.ShareDialogFeature.SHARE_DIALOG)) {
			Log.i(TAG, "canPresent");
			FacebookDialog shareDialog = new FacebookDialog.ShareDialogBuilder(activity)
					.setCaption(caption)
					.setDescription(description)
					.setName(name)
					.setLink(link)
					.setPicture(picture)
					.build();
			uiHelper.trackPendingDialogCall(shareDialog.present());
		} else {
			Log.i(TAG, "webdialog");
			publishFeedDialog();
		}
	}

	private void publishFeedDialog() {
		Bundle params = new Bundle();

		Log.i("name", name);
		Log.i("caption",caption);
		Log.i("description",description);
		Log.i("link",link);
		Log.i("picture",picture);

		params.putString("name", name);
		params.putString("caption",caption);
		params.putString("description",description);
		params.putString("link",link);
		params.putString("picture",picture);

		WebDialog feedDialog = (new WebDialog.FeedDialogBuilder(activity, Session.getActiveSession(), params)).setOnCompleteListener(new OnCompleteListener() {

			@Override
			public void onComplete(Bundle values, FacebookException error) {
				if (error == null) {
					// When the story is posted, echo the success
					// and the post Id.
					final String postId = values.getString("post_id");
					if (postId != null) {
						Toast.makeText(activity, "Posted story, id: " + postId, Toast.LENGTH_SHORT).show();
					} else {
						// User clicked the Cancel button
						Toast.makeText(activity.getApplicationContext(), "Publish cancelled", Toast.LENGTH_SHORT).show();
					}
				} else if (error instanceof FacebookOperationCanceledException) {
					// User clicked the "x" button
					Toast.makeText(activity.getApplicationContext(), "Publish cancelled", Toast.LENGTH_SHORT).show();
				} else {
					// Generic, ex: network error
					Toast.makeText(activity.getApplicationContext(), "Error posting story", Toast.LENGTH_SHORT).show();
				}
			}

		}).build();

		feedDialog.show();
	}

	private class SessionStatusCallback implements Session.StatusCallback, FacebookDialog.Callback {

		@Override
		public void call(Session session, SessionState state, Exception exception) {
			onSessionStateChange(session, state, exception);
		}

		@Override
		public void onComplete(PendingCall pendingCall, Bundle data) {
			Log.i(TAG, "ShareDialog onComplete");
			boolean didCancel = FacebookDialog.getNativeDialogDidComplete(data);
			String completionGesture = FacebookDialog.getNativeDialogCompletionGesture(data);
			String postId = FacebookDialog.getNativeDialogPostId(data);
			Log.i(TAG, "didCancel:" + didCancel + "completionGesture:" + completionGesture + "postId:" + postId);
		}

		@Override
		public void onError(PendingCall pendingCall, Exception error, Bundle data) {
			Log.i(TAG, "ShareDialog onError");
			boolean didCancel = FacebookDialog.getNativeDialogDidComplete(data);
			String completionGesture = FacebookDialog.getNativeDialogCompletionGesture(data);
			String postId = FacebookDialog.getNativeDialogPostId(data);
			Log.i(TAG, "didCancel:" + didCancel + "completionGesture:" + completionGesture + "postId:" + postId);
		}
	}

	private void onSessionStateChange(Session session, SessionState state, Exception exception) {
		if (state.isOpened()) {
			Log.i(TAG, "Logged in...");
		} else if (state.isClosed()) {
			Log.i(TAG, "Logged out...");
		}
	}

	public void onCreate(Bundle savedInstanceState) {
		uiHelper.onCreate(savedInstanceState);
	}

	public void onResume() {
		Session session = Session.getActiveSession();
		if (session != null && (session.isOpened() || session.isClosed())) {
			onSessionStateChange(session, session.getState(), null);
		}
		uiHelper.onResume();
	}

	public void onPause() {
		uiHelper.onPause();
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		uiHelper.onActivityResult(requestCode, resultCode, data, dialogCallback);
	}

	public void onSaveInstanceState(Bundle outState) {
		uiHelper.onSaveInstanceState(outState);
	}

	public void onDestroy() {
		uiHelper.onDestroy();
	}

	private FacebookDialog.Callback dialogCallback = new FacebookDialog.Callback() {
		@Override
		public void onError(FacebookDialog.PendingCall pendingCall,
				Exception error, Bundle data) {
			Log.d("HelloFacebook", String.format("Error: %s", error.toString()));
		}

		@Override
		public void onComplete(FacebookDialog.PendingCall pendingCall,
				Bundle data) {
			Log.d("HelloFacebook", "Success!");
		}
	};
}
