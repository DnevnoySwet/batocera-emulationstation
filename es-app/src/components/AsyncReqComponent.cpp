#include "components/AsyncReqComponent.h"

#include "HttpReq.h"
#include "Renderer.h"
#include "LocaleES.h"

AsyncReqComponent::AsyncReqComponent(Window* window, std::shared_ptr<HttpReq> req, std::function<void(std::shared_ptr<HttpReq>)> onSuccess, std::function<void()> onCancel) 
	: GuiComponent(window), 
	mSuccessFunc(onSuccess), mCancelFunc(onCancel), mTime(0), mRequest(req)
{
	
}

bool AsyncReqComponent::input(InputConfig* config, Input input)
{
	if(input.value != 0 && config->isMappedTo("a", input)) // batocera
	{
		if(mCancelFunc)
			mCancelFunc();

		delete this;
	}

	return true;
}

void AsyncReqComponent::update(int deltaTime)
{
	if(mRequest->status() != HttpReq::REQ_IN_PROGRESS)
	{
		mSuccessFunc(mRequest);
		delete this;
		return;
	}

	mTime += deltaTime;
}

void AsyncReqComponent::render(const Transform4x4f& /*parentTrans*/)
{
	Transform4x4f trans = Transform4x4f::Identity();
	trans = trans.translate(Vector3f(Renderer::getScreenWidth() / 2.0f, Renderer::getScreenHeight() / 2.0f, 0));
	Renderer::setMatrix(trans);

	Vector3f point(Math::cosf(mTime * 0.01f) * 12, Math::sinf(mTime * 0.01f) * 12, 0);
	Renderer::drawRect((int)point.x(), (int)point.y(), 8, 8, 0x0000FFFF);
}

std::vector<HelpPrompt> AsyncReqComponent::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("a", _("CANCEL"))); // batocera
	return prompts;
}
