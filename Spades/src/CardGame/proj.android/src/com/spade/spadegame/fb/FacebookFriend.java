package com.spade.spadegame.fb;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

import com.facebook.FacebookException;
import com.facebook.Session;
import com.facebook.SessionState;
import com.facebook.UiLifecycleHelper;
import com.facebook.widget.WebDialog;

public class FacebookFriend {

	private static final String TAG = "FacebookFriend";

	private static final int MSG_INVITE_FRIEND = 10;

	private static FacebookFriend instance;
	private UiLifecycleHelper uiHelper;
	private Session.StatusCallback statusCallback = new SessionStatusCallback();
	private Activity activity;
	boolean pickFriendsWhenSessionOpened;
	boolean pickFriendWhenSessionUpdated;

	private WebDialog dialog;

	public static int action;

	private static final Handler mHandler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case MSG_INVITE_FRIEND:
				FacebookFriend.action = msg.arg1;
				instance.performPickFriend();
				break;
			}
		}
	};

	public FacebookFriend(Activity activity) {
		this.activity = activity;
		instance = this;

		try {
			uiHelper = new UiLifecycleHelper(activity, statusCallback);
		} catch (Error e) {
			Log.e(TAG, "LifecycleHelper Error", e);
		} catch (Exception e) {
			Log.e(TAG, "LifecycleHelper Error", e);
		}
	}

	public void release() {
		instance = null;
	}

	public static void nativeInviteFriend(int action) {
		Message msg = new Message();
		msg.what = MSG_INVITE_FRIEND;
		msg.arg1 = action;
		mHandler.sendMessage(msg);
	}

	public void performPickFriend() {
		if (ensureOpenSession()) {
			Bundle params = new Bundle();
			params.putString("message", "Could you play spade game? I hope play game with you.");
			showDialogWithoutNotificationBar("apprequests", params);
		}
	}

	private void showDialogWithoutNotificationBar(String action, Bundle params) {  
		dialog = new WebDialog.Builder(activity, Session.getActiveSession(), action, params).setOnCompleteListener(new WebDialog.OnCompleteListener() {
			@Override
			public void onComplete(Bundle values, FacebookException error) {
				if (error != null && error.getMessage() != null) {
					Log.e(TAG, error.getMessage());
				}
				dialog = null;
			}
		}).build();

		Window dialogWindow = dialog.getWindow();
		dialogWindow.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

		dialog.show();
	}

	private boolean ensureOpenSession() {
		Session session = Session.getActiveSession();
		if (session == null || !session.isOpened()) {
			Session.openActiveSession(activity, true, statusCallback);
			pickFriendsWhenSessionOpened = true;
			return false;
		}

		if (!session.getPermissions().contains("user_friends")) {
			try {
				requestFriendsPermission(session);
				pickFriendWhenSessionUpdated = true;
			} catch (Exception e) {
				pickFriendWhenSessionUpdated = false;
				Log.e(TAG, "error", e);
				Toast.makeText(activity, "Cannot invite user", Toast.LENGTH_SHORT).show();
			}
			return false;
		}
		return true;
	}

	private void requestFriendsPermission(Session session) {
		Session.NewPermissionsRequest reqeust = new Session.NewPermissionsRequest(activity, "user_friends");
		session.addCallback(statusCallback);
		session.requestNewReadPermissions(reqeust);
	}

	private void onSessionStateChange(Session session, SessionState state, Exception exception) {
		if (state.isOpened()) {
			Log.i(TAG, "Logged in...");
			if (pickFriendsWhenSessionOpened) {
				pickFriendsWhenSessionOpened = false;
				try {
					requestFriendsPermission(session);
					pickFriendWhenSessionUpdated = true;
				} catch (Exception e) {
					pickFriendWhenSessionUpdated = false;
				}
				return;
			}
		} else if (state.isClosed()) {
			Log.i(TAG, "Logged out...");
		}

		if (pickFriendWhenSessionUpdated && state == SessionState.OPENED_TOKEN_UPDATED) {
			pickFriendWhenSessionUpdated = false;
			performPickFriend();
		}
	}

	public void onCreate(Bundle savedInstanceState) {
		if (uiHelper != null) {
			uiHelper.onCreate(savedInstanceState);
		}
	}

	public void onResume() {
		Session session = Session.getActiveSession();
		if (session != null && (session.isOpened() || session.isClosed())) {
			onSessionStateChange(session, session.getState(), null);
		}

		if (uiHelper != null) {
			uiHelper.onResume();
		}
	}

	public void onPause() {
		if (uiHelper != null) {
			uiHelper.onPause();
		}
	}

	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (uiHelper != null) {
			uiHelper.onActivityResult(requestCode, resultCode, data);
		}
	}

	public void onSaveInstanceState(Bundle outState) {
		if (uiHelper != null) {
			uiHelper.onSaveInstanceState(outState);
		}
	}

	public void onDestroy() {
		if (uiHelper != null) {
			uiHelper.onDestroy();
		}
	}

	private class SessionStatusCallback implements Session.StatusCallback {
		@Override
		public void call(Session session, SessionState state, Exception exception) {
			// Respond to session state changes, ex: updating the view
			onSessionStateChange(session, state, exception);
		}
	}
}
