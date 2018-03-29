package com.spade.spadegame;

import android.app.Application;
import com.facebook.model.GraphUser;

import java.util.List;

public class SpadeApplication extends Application {

	private List<GraphUser> selectedUsers;

	public List<GraphUser> getSelectedUsers() {
		return selectedUsers;
	}

	public void setSelectedUsers(List<GraphUser> selectedUsers) {
		this.selectedUsers = selectedUsers;
	}
}
